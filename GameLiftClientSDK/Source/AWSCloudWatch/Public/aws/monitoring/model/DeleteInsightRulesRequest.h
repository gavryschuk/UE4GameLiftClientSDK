﻿/*
* Copyright 2010-2017 Amazon.com, Inc. or its affiliates. All Rights Reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License").
* You may not use this file except in compliance with the License.
* A copy of the License is located at
*
*  http://aws.amazon.com/apache2.0
*
* or in the "license" file accompanying this file. This file is distributed
* on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
* express or implied. See the License for the specific language governing
* permissions and limitations under the License.
*/

#pragma once
#include <aws/monitoring/CloudWatch_EXPORTS.h>
#include <aws/monitoring/CloudWatchRequest.h>
#include <aws/core/utils/memory/stl/AWSVector.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace CloudWatch
{
namespace Model
{

  /**
   */
  class AWS_CLOUDWATCH_API DeleteInsightRulesRequest : public CloudWatchRequest
  {
  public:
    DeleteInsightRulesRequest();

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "DeleteInsightRules"; }

    Aws::String SerializePayload() const override;

  protected:
    void DumpBodyToUrl(Aws::Http::URI& uri ) const override;

  public:

    /**
     * <p>An array of the rule names to delete. If you need to find out the names of
     * your rules, use <a>DescribeInsightRules</a>.</p>
     */
    inline const Aws::Vector<Aws::String>& GetRuleNames() const{ return m_ruleNames; }

    /**
     * <p>An array of the rule names to delete. If you need to find out the names of
     * your rules, use <a>DescribeInsightRules</a>.</p>
     */
    inline bool RuleNamesHasBeenSet() const { return m_ruleNamesHasBeenSet; }

    /**
     * <p>An array of the rule names to delete. If you need to find out the names of
     * your rules, use <a>DescribeInsightRules</a>.</p>
     */
    inline void SetRuleNames(const Aws::Vector<Aws::String>& value) { m_ruleNamesHasBeenSet = true; m_ruleNames = value; }

    /**
     * <p>An array of the rule names to delete. If you need to find out the names of
     * your rules, use <a>DescribeInsightRules</a>.</p>
     */
    inline void SetRuleNames(Aws::Vector<Aws::String>&& value) { m_ruleNamesHasBeenSet = true; m_ruleNames = std::move(value); }

    /**
     * <p>An array of the rule names to delete. If you need to find out the names of
     * your rules, use <a>DescribeInsightRules</a>.</p>
     */
    inline DeleteInsightRulesRequest& WithRuleNames(const Aws::Vector<Aws::String>& value) { SetRuleNames(value); return *this;}

    /**
     * <p>An array of the rule names to delete. If you need to find out the names of
     * your rules, use <a>DescribeInsightRules</a>.</p>
     */
    inline DeleteInsightRulesRequest& WithRuleNames(Aws::Vector<Aws::String>&& value) { SetRuleNames(std::move(value)); return *this;}

    /**
     * <p>An array of the rule names to delete. If you need to find out the names of
     * your rules, use <a>DescribeInsightRules</a>.</p>
     */
    inline DeleteInsightRulesRequest& AddRuleNames(const Aws::String& value) { m_ruleNamesHasBeenSet = true; m_ruleNames.push_back(value); return *this; }

    /**
     * <p>An array of the rule names to delete. If you need to find out the names of
     * your rules, use <a>DescribeInsightRules</a>.</p>
     */
    inline DeleteInsightRulesRequest& AddRuleNames(Aws::String&& value) { m_ruleNamesHasBeenSet = true; m_ruleNames.push_back(std::move(value)); return *this; }

    /**
     * <p>An array of the rule names to delete. If you need to find out the names of
     * your rules, use <a>DescribeInsightRules</a>.</p>
     */
    inline DeleteInsightRulesRequest& AddRuleNames(const char* value) { m_ruleNamesHasBeenSet = true; m_ruleNames.push_back(value); return *this; }

  private:

    Aws::Vector<Aws::String> m_ruleNames;
    bool m_ruleNamesHasBeenSet;
  };

} // namespace Model
} // namespace CloudWatch
} // namespace Aws
